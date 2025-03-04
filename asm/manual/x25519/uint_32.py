def hex_to_uint32_array(hex_str):
    hex_str = hex_str.strip().replace(" ", "").replace("\n", "")

    chunks = [hex_str[i:i+8] for i in range(0, len(hex_str), 8)]

    result = []
    for chunk in chunks:
        little_endian = "".join(reversed([chunk[i:i+2] for i in range(0, 8, 2)]))
        result.append(f"0x{little_endian}")

    return ", ".join(result)

scalar = "77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a"
uint32_format = hex_to_uint32_array(scalar)
print(f"uint32_t private[X25519_SIZE] = {{ {uint32_format} }};")
u_cor = " 09 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0000"
uint32_format = hex_to_uint32_array(u_cor)
print(f"uint32_t public[X25519_SIZE] = {{ {uint32_format} }};")
outputexcept = "8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a"
uint32_format = hex_to_uint32_array(outputexcept)
print(f"uint32_t public[X25519_SIZE] = {{ {uint32_format} }};")