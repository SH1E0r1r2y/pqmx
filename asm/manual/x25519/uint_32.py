def hex_to_uint32_array(hex_str):
    hex_str = hex_str.strip().replace(" ", "").replace("\n", "")

    chunks = [hex_str[i:i+8] for i in range(0, len(hex_str), 8)]

    result = []
    for chunk in chunks:
        little_endian = "".join(reversed([chunk[i:i+2] for i in range(0, 8, 2)]))
        result.append(f"0x{little_endian}")

    return ", ".join(result)

scalar = "a546e36bf0527c9d3b16154b82465edd62144c0ac1fc5a18506a2244ba449ac4"
uint32_format = hex_to_uint32_array(scalar)
print(f"uint32_t private[X25519_SIZE] = {{ {uint32_format} }};")
u_cor = "e6db6867583030db3594c1a424b15f7c726624ec26b3353b10a903a6d0ab1c4c"
uint32_format = hex_to_uint32_array(u_cor)
print(f"uint32_t public[X25519_SIZE] = {{ {uint32_format} }};")
outputexcept = "c3da55379de9c6908e94ea4df28d084f32eccf03491c71f754b4075577a28552"
uint32_format = hex_to_uint32_array(outputexcept)
print(f"uint32_t public[X25519_SIZE] = {{ {uint32_format} }};")