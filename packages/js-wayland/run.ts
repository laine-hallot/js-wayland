class ClientStateStruct {
  client_state_ptr: c_ptr;
  wl_display: c_ptr = $SHBuiltin.c_null();
  wl_registry: c_ptr = $SHBuiltin.c_null();
  wl_surface: c_ptr = $SHBuiltin.c_null();
  xdg_wm_base: c_ptr = $SHBuiltin.c_null();
  xdg_surface: c_ptr = $SHBuiltin.c_null();
  xdg_toplevel: c_ptr = $SHBuiltin.c_null();
  constructor() {
    this.client_state_ptr = _create_heap_client_state();
  }
  set_wl_display(display_ptr: c_ptr) {
    this.wl_display = display_ptr;
    _client_state_add_display(this.client_state_ptr, display_ptr);
  }
  set_wl_registry(registry_ptr: c_ptr) {
    this.wl_registry = registry_ptr;
    _client_state_add_registry(this.client_state_ptr, registry_ptr);
  }

  set_wl_surface(ptr: c_ptr) {
    this.wl_surface = ptr;
    _client_state_add_wl_surface(this.client_state_ptr, ptr);
  }

  set_xdg_surface(ptr: c_ptr) {
    this.xdg_surface = ptr;
    _client_state_add_xdg_surface(this.client_state_ptr, ptr);
  }
  set_xdg_toplevel(ptr: c_ptr) {
    this.xdg_toplevel = ptr;
    _client_state_add_xdg_toplevel(this.client_state_ptr, ptr);
  }
}

let awa = $SHBuiltin.c_null();
const thing = {
  set thing(val: c_ptr) {
    awa = val;
  },
  get thing() {
    return awa;
  },
};

const runWlExample = () => {
  const clientState = new ClientStateStruct();

  const NULL = $SHBuiltin.c_null();
  // @ts-expect-error -- Shermes can handle casting `c_null` to `c_ptr` but that makes the typescript mad
  clientState.set_wl_display(_wl_display_connect(NULL));

  clientState.set_wl_registry(_wl_display_get_registry(clientState.wl_display));

  _attach_registry_listener(clientState.client_state_ptr);

  console.log('waiting for wayland requests');
  _wrapped_roundtrip(clientState.client_state_ptr);
  console.log('wayland requests complete');

  const wl_compositor = _client_state_get_compositor_ptr(
    clientState.client_state_ptr,
  );
  const xdg_wm_base = _client_state_get_xdg_wm_base_ptr(
    clientState.client_state_ptr,
  );

  clientState.set_wl_surface(_wl_compositor_create_surface(wl_compositor));
  clientState.set_xdg_surface(
    _xdg_wm_base_get_xdg_surface(xdg_wm_base, clientState.wl_surface),
  );
  _attach_xdg_surface_listener(clientState.client_state_ptr);

  clientState.set_xdg_toplevel(
    _xdg_surface_get_toplevel(clientState.xdg_surface),
  );
  _xdg_toplevel_set_title(
    clientState.xdg_toplevel,
    stringToAsciiz('Example client'),
  );
  _wl_surface_commit(clientState.wl_surface);
  while (_wl_display_dispatch(clientState.wl_display)) {
    // nothing
  }

  console.log('JS Wayland - DONE');
};
runWlExample();
