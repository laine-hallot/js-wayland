class ClientStateStruct {
  client_state_ptr: c_ptr;
  wl_display: c_ptr = $SHBuiltin.c_null();
  wl_registry: c_ptr = $SHBuiltin.c_null();
  wl_compositor: c_ptr = $SHBuiltin.c_null();
  xdg_wm_base: c_ptr = $SHBuiltin.c_null();
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
}

const runWlExample = () => {
  //const client_state_ptr = _create_heap_client_state();

  const clientState = new ClientStateStruct();

  const NULL = $SHBuiltin.c_null();
  // @ts-expect-error -- Shermes can handle casting `c_null` to `c_ptr` but that makes the typescript mad
  clientState.set_wl_display(_wl_display_connect(NULL));

  //_client_state_add_display(client_state_ptr, wl_display);
  clientState.set_wl_registry(_wl_display_get_registry(clientState.wl_display));

  console.log('waiting for wayland requests');
  //_create_display_registry(clientState.client_state_ptr);
  _attach_registry_and_wait(clientState.client_state_ptr);
  console.log('wayland requests complete');

  //_wrapped_roundtrip(clientState.client_state_ptr);

  console.log('JS Wayland - DONE');
};
runWlExample();
