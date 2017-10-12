class PasswordsClass:

    def __init__(self, correct_password):
        self.password = correct_password

    def verify_password(self, attempt):
        return attempt == self.password
