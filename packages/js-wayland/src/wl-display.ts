const NULL = $SHBuiltin.c_null();

type WlDisplay = {
  wl_display_get_registry: () => void;
};

const createWlDisplay = (ptr: c_ptr) => {
  const wl_display_get_registry = () => {
    const regPtr = _wl_display_get_registry(ptr);
  };

  return { wl_display_get_registry };
};

const wl_display_connect = (): c_ptr => {
  // @ts-expect-error -- Shermes can handle casting `c_null` to `c_ptr` but that makes the typescript mad
  const displayPtr = _wl_display_connect(NULL);
  return displayPtr;
};
