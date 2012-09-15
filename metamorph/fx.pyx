import numpy as np
cimport numpy as np
np.import_array()
from libcpp.vector cimport vector

cimport fx


cdef class FX:
    cdef c_FX* thisptr

    def __cinit__(self):
        self.thisptr = new c_FX()

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr

    property frame_size:
        def __get__(self): return self.thisptr.frame_size()
        def __set__(self, int n): self.thisptr.frame_size(n)

    property hop_size:
        def __get__(self): return self.thisptr.hop_size()
        def __set__(self, int n): self.thisptr.hop_size(n)

    property max_partials:
        def __get__(self): return self.thisptr.max_partials()
        def __set__(self, int n): self.thisptr.max_partials(n)

    property harmonic_scale:
        def __get__(self): return self.thisptr.harmonic_scale()
        def __set__(self, double n): self.thisptr.harmonic_scale(n)

    property residual_scale:
        def __get__(self): return self.thisptr.residual_scale()
        def __set__(self, double n): self.thisptr.residual_scale(n)

    property transient_scale:
        def __get__(self): return self.thisptr.transient_scale()
        def __set__(self, double n): self.thisptr.transient_scale(n)

    property harmonic_distortion:
        def __get__(self): return self.thisptr.harmonic_distortion()
        def __set__(self, double n): self.thisptr.harmonic_distortion(n)

    property fundamental_frequency:
        def __get__(self): return self.thisptr.fundamental_frequency()
        def __set__(self, double n): self.thisptr.fundamental_frequency(n)

    property env_interp:
        def __get__(self): return self.thisptr.env_interp()
        def __set__(self, double n): self.thisptr.env_interp(n)

    def apply_envelope(self, np.ndarray[dtype_t, ndim=1] env):
        self.thisptr.apply_envelope(len(env), <double*> env.data)

    def clear_envelope(self):
        self.thisptr.clear_envelope()

    def process_frame(self, np.ndarray[dtype_t, ndim=1] audio):
        cdef np.ndarray[dtype_t, ndim=1] output = np.zeros(len(audio))
        self.thisptr.process_frame(len(audio), <double*> audio.data,
                                   len(output), <double*> output.data)
        return output

    def process(self, np.ndarray[dtype_t, ndim=1] audio):
        cdef np.ndarray[dtype_t, ndim=1] output = np.zeros(len(audio))
        self.thisptr.process(len(audio), <double*> audio.data,
                             len(output), <double*> output.data)
        return output


cdef class TimeScale(FX):
    def __cinit__(self):
        if self.thisptr:
            del self.thisptr
        self.thisptr = new c_TimeScale()

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <c_TimeScale*>0

    property scale_factor:
        def __get__(self): return (<c_TimeScale*>self.thisptr).scale_factor()
        def __set__(self, double n): (<c_TimeScale*>self.thisptr).scale_factor(n)

    def process(self, np.ndarray[dtype_t, ndim=1] audio):
        cdef np.ndarray[dtype_t, ndim=1] output = np.zeros(len(audio) * self.scale_factor)
        self.thisptr.process(len(audio), <double*> audio.data,
                             len(output), <double*> output.data)
        return output


cdef class SpectralEnvelope:
    cdef c_SpectralEnvelope* thisptr

    def __cinit__(self, int order, int env_size):
        if self.thisptr:
            del self.thisptr
        self.thisptr = new c_SpectralEnvelope(order, env_size)
    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <c_SpectralEnvelope*>0

    def env(self, np.ndarray[dtype_t, ndim=1] freqs,
                  np.ndarray[dtype_t, ndim=1] mags):
        cdef np.ndarray[dtype_t, ndim=1] env = \
            np.zeros(self.thisptr.env_size())
        self.thisptr.env(len(freqs), <double*> freqs.data, <double*> mags.data,
                         len(env), <double*> env.data)
        return env
