from conans import ConanFile

class WyllisDetectorConan(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake'

    def requirements(self):
        self.requires('OpenCV/3.3.0@piponazo/testing')