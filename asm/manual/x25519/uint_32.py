def hex_to_uint32_array(hex_str):
    hex_str = hex_str.strip().replace(" ", "").replace("\n", "")

    chunks = [hex_str[i:i+8] for i in range(0, len(hex_str), 8)]

    result = []
    for chunk in chunks:
        little_endian = "".join(reversed([chunk[i:i+2] for i in range(0, 8, 2)]))
        result.append(f"0x{little_endian}")

    return ", ".join(result)

alice_public = "41b6ec3c50ee7af203c0026e5e079e7fa8cbc9bc581d49cb0d537d5778497c87"
uint32_format = hex_to_uint32_array(alice_public)
print(f"uint32_t publickey[X25519_SIZE] = {{ {uint32_format} }};")
