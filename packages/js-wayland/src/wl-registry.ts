type WlRegistryListener = {
  global: (
    data: c_ptr,
    registry: WlRegistry,
    name: c_ptr, // c_int,
    interfacePtr: c_ptr, // c_char,
    version: c_ptr, // c_int,
  ) => void;
  global_remove: (
    data: c_ptr,
    registry: WlRegistry,
    name: c_ptr, // c_int,
  ) => void;
};

type WlRegistry = {
  attach_registry_listener: (clientState: any) => void;
};

const createWlRegistry = (ptr: c_ptr) => {
  const attach_registry_listener = (clientState: any): void => {
    _attach_registry_listener(clientState.client_state_ptr);
  };

  return { attach_registry_listener };
};
