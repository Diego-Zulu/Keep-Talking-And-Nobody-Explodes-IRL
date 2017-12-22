import time
import threading
from .lcd_controller import LcdController
from multiprocessing import Process
from ..songs import SoundEffectPlayer

class CountdownTimer(threading.Thread):

    def __init__(self, time_in_seconds, x, to_lcd):
        self.start_time = time_in_seconds
        self.actual_time = time_in_seconds
        self.formatted_time = self.format_time(time_in_seconds)
        self.second_left = time_in_seconds
        self.stop = False
        self.lose = x
        self.to_lcd_array = to_lcd
        threading.Thread.__init__(self)

    def run(self):
        self.countdown()
        
    def countdown(self):
        while self.actual_time > 0:
            #time.sleep(0.50)
            time.sleep(0.90)
            self.actual_time -= 1
            self.update_screen()
            if self.stop:
                break
        if not(self.stop) and self.actual_time <= 0:
            self.lose()

    def current_countdown(self):
        return int(self.actual_time)

    def current_countdown_formater(self):
        return self.format_time(int(self.actual_time))

    def format_time(self, time):
        mins, secs = divmod(self.actual_time, 60)
        timeformat = '{:02d}:{:02d}'.format(mins, secs)
        return timeformat
    
    def update_screen(self):
        form_time = self.current_countdown_formater()
        l = self.to_lcd_array()
        l0 = "[{0}] {1}".format(form_time, l[0])
        LcdController.print_line_1(l0)
        LcdController.print_line_2(l[1])
        SoundEffectPlayer.play_beep()
