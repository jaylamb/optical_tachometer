from tkinter import ttk

import serialtach as tach
import sys
import time
import tkinter as tk

class Optical_Tachometer_GUI:
    units = 'Hz'
    data_str = ''

    def __init__( self, tk_root ):
        tk_root.title('Optical Tachometer Demo')
        tk_root.config( background = 'black' )
        tk_root.geometry('264x150')
        tk_root.resizable( False, False )

        style = ttk.Style()
        style.theme_use('clam')
        style.configure(
            'TButton',
            foreground = 'black',
            font       = ( 'Arial', 11, 'bold' ),
            padding    = 12
        )
        style.map('TButton', foreground = [('pressed', 'orange')])

        self.title_text = ttk.Label(
            tk_root,
            text       = 'Optical Tachometer Demo',
            foreground = 'lawn green',
            background = 'black',
            font       = ( 'Arial', 14 ),
            padding    = 5
        )
        self.title_text.grid( row = 0, column = 0, columnspan = 2 )

        self.reading_text = ttk.Label(
            tk_root,
            text       = 'Initializing...',
            foreground = 'lawn green',
            background = 'black',
            font       = ( 'Arial', 18, 'bold' ),
            padding    = 5
        )
        self.reading_text.grid( row = 1, column = 0, columnspan = 2 )

        self.button_frame = ttk.Frame(
            tk_root,
            padding = (5, 5),
            relief ='sunken'
        )
        self.button_frame.grid( row = 2, column = 0, columnspan = 2, padx = 5, pady = 5)

        self.quit_button = ttk.Button(
            self.button_frame,
            text    = 'Quit',
            command = tk_root.destroy
        )
        self.quit_button.grid( row = 2, column = 0, padx = 2 )

        self.change_unit_button = ttk.Button(
            self.button_frame,
            text    = '[RPM]',
            command = self.change_units
        )
        self.change_unit_button.grid( row = 2, column = 1, padx = 2 )

    def refresh_gui( self, tk_root, data = '' ):
        if ( data ):
            if( data != '.' ):
                hz_float = float( data )
                rpm_int  = int( hz_float * 60 )
                data_num = str( hz_float ) if self.units == 'Hz' else str( rpm_int )
                self.data_str = f'{data_num} {self.units}'
            else:
                self.data_str = '< undetected >'

            try:
                self.reading_text.config( text = self.data_str )
            except:
                sys.exit()

    def change_units( self ):
        if( self.units == 'Hz' ):
            self.units = 'RPM'
            self.change_unit_button.config( text = '[Hz]' )
        else:
            self.units = 'Hz'
            self.change_unit_button.config( text = '[RPM]' )

def main():
    root = tk.Tk()
    gui = Optical_Tachometer_GUI( root )

    serial_tach = tach.Serial_Tachometer()
    serial_port = serial_tach.serial_connect()

    while( True ):
        current_data_hz = serial_tach.read_serial_data( serial_port )
        gui.refresh_gui( root, current_data_hz )
        try:
            root.update_idletasks()
            root.update()
        except:
            sys.exit()
        time.sleep( 0.25 )

if __name__ == '__main__': main()
