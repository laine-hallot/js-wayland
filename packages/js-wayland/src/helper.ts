const _handleTable = new Map<number, any>();
let _nextHandle = 1;

function createHandle(obj: any): number {
  const handle = _nextHandle++;
  _handleTable.set(handle, obj);
  return handle;
}

function getHandle(handle: number): any {
  return _handleTable.get(handle);
}

function destroyHandle(handle: number): void {
  _handleTable.delete(handle);
}

globalThis.JsWayland = {
  connect_to_env: () => _connect_to_env(),
  draw_window: () => _draw_window(),
  wl_display_dispatch: (display: c_ptr) => _wl_display_dispatch(display),
  wl_display_connect: (name: string | null): c_ptr => {
    if (name === null) {
      console.log("No display name provided");
      const NULL = $SHBuiltin.c_null();
      // @ts-expect-error -- Shermes can handle casting `c_null` to `c_ptr` but that makes the typescript mad
      return _wl_display_connect(NULL);
    }

    return _wl_display_connect(stringToAsciiz(name));
  },
  wl_display_get_registry: (display: c_ptr) => {
    return _wl_display_get_registry(display);
  },
  attach_registry_listener: (state: {
    wl_display: c_ptr;
    wl_registry: c_ptr;
    wl_shm: c_ptr;
    wl_compositor: c_ptr;
    xdg_wm_base: c_ptr;
    wl_surface: c_ptr;
    xdg_surface: c_ptr;
    xdg_toplevel: c_ptr;
  }) => {
    console.log("attach_registry_listener");
    const stateHandle = createHandle(state);
    console.log("created state handle");
    // @ts-expect-error -- Shermes can handle casting `number` to `c_int` but that makes the typescript mad
    const statePtr = _int_to_ptr(stateHandle);
    console.log("got state pointer");
    _attach_registry_listener(statePtr);
  },
};
//$SHBuiltin.export("JsWayland", thing);
