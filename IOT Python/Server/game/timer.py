import time
import threading
from multiprocessing import Process

class CountdownTimer(threading.Thread):

    def __init__(self, time_in_seconds):
        self.start_time = time_in_seconds
        self.actual_time = time_in_seconds
        self.formatted_time = self.format_time(time_in_seconds)
        self.second_left = time_in_seconds
        threading.Thread.__init__(self)

    def run(self):
        self.countdown()

    def countdown(self):
        while self.actual_time >= 60:
            time.sleep(1)
            self.actual_time -= 1
        while self.actual_time > 0:
            time.sleep(0.01)
            self.actual_time -= 0.01

    def current_countdown(self):
        return int(self.actual_time)

    def current_countdown_formater(self):
        return self.format_time(int(self.actual_time))

    def format_time(self, time):
        if time >= 60:
            mins, secs = divmod(self.actual_time, 60)
            timeformat = '{:02d}:{:02d}'.format(mins, secs)
        else:
            secs, mils = int(self.actual_time), int((self.actual_time-int(self.actual_time))*100)
            timeformat = '{:02d}:{:02d}'.format(secs, mils)
        return timeformat
