type State = {
  wl_display: c_ptr;
  wl_registry: c_ptr;
  wl_shm: c_ptr;
  //wl_compositor: c_ptr;
  //xdg_wm_base: c_ptr;
  wl_surface: c_ptr;
  xdg_surface: c_ptr;
  xdg_toplevel: c_ptr;
};

const waitForRoundTrip = (
  state: State,
  client_state_ptr: c_ptr,
): {
  wl_compositor: c_ptr;
  xdg_wm_base: c_ptr;
} => {
  console.log('about to call roundtrip');
  _wrapped_roundtrip(state.wl_display);
  console.log('roundtrip complete');

  const wl_compositor = _client_state_get_compositor_ptr(client_state_ptr);
  const xdg_wm_base = _client_state_get_xdg_wm_base_ptr(client_state_ptr);
  return { wl_compositor, xdg_wm_base };
};

const runWlExample = () => {
  console.log('meow');
  //globalThis.JsWayland.draw_window();
  const state: State = {
    wl_display: $SHBuiltin.c_null(),
    wl_registry: $SHBuiltin.c_null(),
    wl_shm: $SHBuiltin.c_null(),
    wl_surface: $SHBuiltin.c_null(),
    xdg_surface: $SHBuiltin.c_null(),
    xdg_toplevel: $SHBuiltin.c_null(),
  };
  const client_state_ptr = _create_heap_client_state();

  state.wl_display = globalThis.JsWayland.wl_display_connect(null);
  _client_state_add_display(client_state_ptr, state.wl_display);
  //console.log(state.wl_display);

  state.wl_registry = _wl_display_get_registry(state.wl_display);
  _client_state_add_registry(client_state_ptr, state.wl_registry);

  _attach_registry_listener(client_state_ptr);

  _wrapped_roundtrip(client_state_ptr);

  /*   let wl_compositor: c_ptr = $SHBuiltin.c_null();
  let xdg_wm_base: c_ptr = $SHBuiltin.c_null();

  _wrapped_roundtrip_and_get_globals(
    client_state_ptr,
    wl_compositor,
    xdg_wm_base,
  );

  const client_state_ptr = _create_and_init_client_state();
  const wl_compositor = _client_state_get_compositor_ptr(client_state_ptr);
  const xdg_wm_base = _client_state_get_xdg_wm_base_ptr(client_state_ptr); */

  /*  const { wl_compositor, xdg_wm_base } = waitForRoundTrip(
    state,
    client_state_ptr,
  ); */

  /*   state.wl_surface = _wl_compositor_create_surface(wl_compositor);
  state.xdg_surface = _xdg_wm_base_get_xdg_surface(
    xdg_wm_base,
    state.wl_surface,
  );

  _attach_xdg_surface_listener(client_state_ptr); */
  /*

  state.xdg_toplevel = _xdg_surface_get_toplevel(state.xdg_surface);
  _xdg_toplevel_set_title(state.xdg_toplevel, stringToAsciiz('Example client'));
  _wl_surface_commit(state.wl_surface);

  while (_wl_display_dispatch(state.wl_display)) {
    // nothing
  } */
};
runWlExample();
