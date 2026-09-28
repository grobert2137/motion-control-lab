## Script that converts
#%%
def voltage_to_digital(): ## This might still be wrong in certain edge cases
    range = 2 * int(input('Input the voltage max/min: '))
    voltage = float(input('Input the voltage you want: '))
    output = int((voltage/range * (65535) + 32768))
    output_hex = hex(output)
    print(f"Output: {output} aka {output_hex}")

def hex_to_decimal(hex_val):
    output = int(hex_val)
    return print(f"Output:  {output}")

if __name__ == "__main__":
    hex_to_decimal(0x97ff)
    ##voltage_to_digital
    