from conans import ConanFile

class WyllisDetectorConan(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake'
    default_options = "OpenCV:webcam=True"

    def configure(self):
        self.options['OpenCV'].shared = True
        self.options['OpenCV'].ffmpeg = True
        self.options['OpenCV'].webcam = True

    def requirements(self):
        self.requires('OpenCV/3.3.0@piponazo/testing')

    def imports(self):
        self.copy("*.dll", src="x64/vc15/bin", dst="bin")
