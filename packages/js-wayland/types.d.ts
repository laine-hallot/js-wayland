type BaseCTypes = c_char | c_int | c_null | c_ptr | c_uchar | c_schar;
type FunctionBinding = (...args: any[]) => BaseCTypes | void;
type ExternOptions = {
  include?: string;
  declared?: boolean;
};
type State = {
  wl_display: c_ptr;
  wl_registry: c_ptr;
  wl_shm: c_ptr;
  wl_compositor: c_ptr;
  xdg_wm_base: c_ptr;
  wl_surface: c_ptr;
  xdg_surface: c_ptr;
  xdg_toplevel: c_ptr;
};
declare global {
  type c_char = { __kind: "c_char" };
  type c_int = { __kind: "c_int" };
  type c_null = { __kind: "c_null" };
  type c_ptr = { __kind: "c_ptr" };
  type c_uchar = { __kind: "c_uchar" };
  type c_schar = { __kind: "c_schar" };

  const $SHBuiltin: {
    c_null: () => c_null;
    extern_c: <T extends FunctionBinding>(
      options: ExternOptions,
      binding: T
    ) => T;
    export: (name: string, value: any) => void;
    import: (arg1: number, value: any) => void;
  };
  namespace globalThis {
    var JsWayland: {
      connect_to_env: () => c_int;
      draw_window: () => c_int;
      wl_display_dispatch: (_display: c_ptr) => c_int;
      wl_display_connect: (name: string | null) => c_ptr;
      wl_display_get_registry: (display: c_ptr) => c_ptr;
      attach_registry_listener: (state: State) => void;
    };
  }
}

export {};
