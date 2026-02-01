/* type CChar = string;
type CInt = number;

const CPtr = <T extends (CChar | CInt), U>() => {
  const ptr: c_ptr;
  const readValue = ((val: T) => U) => U;
}; */
