from conans import ConanFile

class WyllisDetectorConan(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake'

    def configure(self):
        self.options['OpenCV'].shared = True
        self.options['OpenCV'].ffmpeg = True
        self.options['OpenCV'].webcam = True

    def requirements(self):
        self.requires('OpenCV/3.3.0@piponazo/testing')
