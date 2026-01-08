type BaseCTypes = c_char | c_int | c_null | c_ptr | c_uchar | c_schar;
type FunctionBinding = (...args: any[]) => BaseCTypes | void;
type ExternOptions = {
  include?: string;
  declared?: boolean;
};

declare global {
  type c_char = { __kind: 'c_char' };
  type c_int = { __kind: 'c_int' };
  type c_null = { __kind: 'c_null' };
  type c_ptr = { __kind: 'c_ptr' };
  type c_uchar = { __kind: 'c_uchar' };
  type c_schar = { __kind: 'c_schar' };

  const $SHBuiltin: {
    c_null: () => c_null;
    extern_c: <T extends FunctionBinding>(
      options: ExternOptions,
      binding: T,
    ) => T;
    export: (name: string, value: any) => void;
    import: (arg1: number, value: any) => void;
  };
}

export {};
