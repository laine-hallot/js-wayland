const runWlExample = () => {
  console.log('JS Wayland example');
  const client_state_ptr = _create_heap_client_state();

  const NULL = $SHBuiltin.c_null();
  // @ts-expect-error -- Shermes can handle casting `c_null` to `c_ptr` but that makes the typescript mad
  const wl_display = _wl_display_connect(NULL);

  _create_and_init_client_state();

  _client_state_add_display(client_state_ptr, wl_display);

  const wl_registry = _wl_display_get_registry(wl_display);
  _client_state_add_registry(client_state_ptr, wl_registry);

  _attach_registry_listener(client_state_ptr);

  console.log('waiting for wayland requests');
  _wrapped_roundtrip(client_state_ptr);
  console.log('wayland requests complete');

  console.log('JS Wayland - DONE');
};
runWlExample();
