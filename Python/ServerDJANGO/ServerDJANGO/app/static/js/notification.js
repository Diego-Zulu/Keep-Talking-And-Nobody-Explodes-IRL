let _type = {
    SUCCESS: 'success',
    INFO: 'info',
    WARNING: 'warning',
    DANGER: 'danger'
};
class Notification {
    static get TYPE() { return _type; }

    static show(type, message) {
        Notification.clear();
        var box = $("#notification_box");
        box.addClass('alert-' + type);
        box.html(message);
        box.show();
    }

    static clear() {
        var box = $("#notification_box");
        box.removeClass('alert-' + _type.SUCCESS);
        box.removeClass('alert-' + _type.INFO);
        box.removeClass('alert-' + _type.WARNING);
        box.removeClass('alert-' + _type.DANGER);
        box.html('');
        box.hide();
    }

    static showTime(type, message, m) {
        Notification.show(type, message);
        setTimeout(function () {
            Notification.clear();
        }, m);
    }
}