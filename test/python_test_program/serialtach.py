import serial
import sys
import time

class Serial_Tachometer:
    def serial_connect( self ):
        try:
            ser = serial.Serial(
                port="/dev/ttyUSB0",
                baudrate=9600,
                timeout = None
            )
        except serial.serialutil.SerialException:
            sys.exit('Serial port connection failure, exiting.')

        ser.reset_input_buffer()
        connection_status = 'connected' if ser else 'nogo'
        print(f'Tach {connection_status} on {ser.name}:', flush = True )

        if( ser ):
            print(ser.get_settings(), flush = True )
            return ser
        else:
            print('Could not connect to serial port.', flush = True )

    def read_serial_data( self, ser ):
        if( ser.in_waiting ):
                data_str = ser.read_until(b':').decode('utf-8')[:-1]
                return data_str

    def print_data( self, data, print_to_console = False ):
        if( data == '.'):
            print('Timed out.', flush = True )
        else:
            hz_float = float(data)
            if( print_to_console ):
                rpm_int = int(hz_float * 60)
                print( f'Hz: {hz_float} RPM: {rpm_int}', flush = True )
            return hz_float

def main():
    tach = Serial_Tachometer()
    serial_port = tach.serial_connect()
    time.sleep( 1 )

    while( True ):
        current_data = tach.read_serial_data( serial_port )
        if( current_data ):
            tach.print_data( current_data, True )

if __name__ == '__main__': main()
