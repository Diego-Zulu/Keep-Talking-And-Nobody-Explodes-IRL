import vlc
import threading

class SoundEffectPlayer(object):
    _vlc_instance = vlc.Instance()
    _player = _vlc_instance.media_player_new()
    _lose_media = _vlc_instance.media_new("file:///home/pi/Documents/ktane-songs/BOMB-LOSE.wav")
    _win_media = _vlc_instance.media_new("file:///home/pi/Documents/ktane-songs/BOMB-WIN.wav")
    _beep_media = _vlc_instance.media_new("file:///home/pi/Documents/ktane-songs/TIME-BEEP.wav")
    _error_media = _vlc_instance.media_new("file:///home/pi/Documents/ktane-songs/MODULE-ERROR.wav")
    _ok_media = _vlc_instance.media_new("file:///home/pi/Documents/ktane-songs/MODULE-OK.wav")
        
    @classmethod
    def play_lose(cls):
        play_song(cls._player, cls._lose_media)
        pass
        
    @classmethod
    def play_win(cls):
        play_song(cls._player, cls._win_media)
    
    @classmethod
    def play_error(cls):
        play_song(cls._player, cls._error_media)
        
    @classmethod
    def play_ok(cls):
        play_song(cls._player, cls._ok_media)
    
    @classmethod
    def play_beep(cls):
        play_song(cls._player, cls._beep_media)
        pass
        
def play_song(player, media):
    try:
        threading.Thread(target=thread_play_song, args=(player, media)).start()
    except:
        print("Error: unable to start thread end song")

def thread_play_song(player, media):
    player.stop()
    media.get_mrl()
    player.set_media(media)
    player.play()
    
