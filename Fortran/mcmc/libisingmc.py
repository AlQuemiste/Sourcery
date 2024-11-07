import numpy as np
import ctypes as ct
import os, time, datetime

# load the shared library
xlib = ct.CDLL(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), 'libisingmc.so'))

#-- define types and structs

i32_t = ct.c_int
f64_t = ct.c_double
nf_t = ct.c_char
nf_ptr_t = ct.c_void_p

i32_arr_t = np.ctypeslib.ndpointer(dtype=i32_t, ndim=1, flags='C_CONTIGUOUS')
f64_arr_t = np.ctypeslib.ndpointer(dtype=f64_t, ndim=1, flags='C_CONTIGUOUS')

# Parameters
class Parameters(ct.Structure):
    _fields_ = [
        ("thermalization", i32_t),
        ("steps", i32_t),
        ("j", i32_t),
        ("t", f64_t),
        ("beta", f64_t),
        ("h", f64_t),
        ("L", i32_t),
    ]

# Observables
class Observables(ct.Structure):
    _fields_ = [
        ("energy", f64_t),
        ("magnetization", f64_t),
        ("specific_heat", f64_t),
        ("susceptibility", f64_t),
        ("mag_localization", f64_t),
    ]

#-- define the argument and return types for the external functions

# IsingMCMC(prm, lattice, energies, magnetizations, obs) bind(C)
# type(Parameters), intent(inout) :: prm
# C_I32, dimension(prm%L, prm%L), intent(inout) :: lattice
# C_F64, dimension(prm%steps), target, intent(out) :: energies, magnetizations
# type(Observables), intent(out) :: obs

xlib.IsingMCMC.argtypes = [
    ct.POINTER(Parameters),
    i32_arr_t,  # lattice
    f64_arr_t,  # energies
    f64_arr_t,  # magnetizations
    f64_arr_t,  # magnetic localizations
    ct.POINTER(Observables)
]

xlib.IsingMCMC.restype = None
#----------------------------------------

class IsingMCMC:

    def __init__(self, prm:Parameters):
        self.prm = prm
        self.lattice = np.empty(self.prm.L ** 2, dtype=i32_t)
        self.energies = np.empty(self.prm.steps, dtype=f64_t)
        self.magnetizations = np.empty_like(self.energies)
        self.mag_localizations = np.empty(self.prm.steps * 2, dtype=f64_t)
        self.observables = Observables()

    def mc(self, prm:Parameters=None):
        if prm is not None:
            self.prm = prm

        IsingMCMC.verifyParameters(self.prm)

        xlib.IsingMCMC(self.prm, self.lattice, self.energies,
                       self.magnetizations, self.mag_localizations,
                       self.observables)

    @staticmethod
    def verifyParameters(prm):
        if prm.L < 2:
            raise ValueError("linear lattice size, {:d}, must be >= 2"
                             .format(prm.L))
        if prm.j < 0:
            prm.j = -1
        else:
            prm.j = +1

    def __str__(self):
        s_ = "IsingMCMC: " + (
            ', '.join(
                ("thermalization = %d" % self.prm.thermalization,
                 "steps = %d" % self.prm.steps,
                 "j = %+d" % int(self.prm.j),
                 "t = %.3e" % self.prm.t,
                 "beta = %.3e" % self.prm.beta,
                 "h = %.3e" % self.prm.h,
                 "L = %d" % self.prm.L))
        )

        return s_

    def results(self):
        s_ = "IsingMCMC results: " + (
            ', '.join(
                ("energy = %.3e" % self.observables.energy,
                 "magnetization = %.3e" % self.observables.magnetization,
                 "specific_heat = %.3e" % self.observables.specific_heat,
                 "susceptibility = %.3e" % self.observables.susceptibility,
                 "magnetic localization = %.3e" % self.observables.mag_localization))
        )

        return s_
#----------------------------------------

def test_IsingMCMC():
    from timeit import default_timer as timer

    prm = Parameters()
    prm.steps = 100000
    prm.thermalization = int(prm.steps * 0.1)
    prm.j = +1
    prm.t = 0.4
    prm.h = 0
    prm.L = 10

    print("> test_IsingMCMC")

    ising_mc = IsingMCMC(prm);
    print(ising_mc)

    t0 = timer()
    ising_mc.mc();
    mc_elapsed = timer() - t0

    print("  Ising MCMC: elapsed time (s): {:.3g}".format(mc_elapsed))
    print(ising_mc.results())
    print("> test_IsingMCMC: DONE.")

def test_multi_IsingMCMC():
    from matplotlib import pyplot as plt

    prm = Parameters()
    prm.steps = 100000
    prm.thermalization = int(prm.steps * 0.1)
    prm.j = +1
    prm.h = 0
    prm.L = 10

    ising_mc = IsingMCMC(prm);

    Ts = np.linspace(1.0, 3.5, 2**4)
    Ms = np.empty_like(Ts)
    Cvs = np.empty_like(Ts)
    Chis = np.empty_like(Ts)
    MLocs = np.empty_like(Ts)

    for i_t, t_ in enumerate(Ts):
        print("{:d}) T = {:.3f}".format(i_t, t_))
        prm.t = t_
        ising_mc.mc(prm)

        print("   m = {:.3f}, c_v = {:.3f}"
              .format(ising_mc.observables.magnetization,
                      ising_mc.observables.specific_heat))

        Ms[i_t] = ising_mc.observables.magnetization
        Cvs[i_t] = ising_mc.observables.specific_heat
        Chis[i_t] = ising_mc.observables.susceptibility
        MLocs[i_t] = ising_mc.observables.mag_localization

    #-- plot
    title = (("2d Ising: L = {:d}, " + r"$J / |J|$" + "= {:+d}, "
              + r"$h / |J|$" + " = {:.3g}, MC measures = {:d}")
              .format(prm.L, prm.j, prm.h, prm.steps))
    xlabel = "T / J"
    fig_filename = "ising_mcmc_l%d" % prm.L

    fig, axs = plt.subplots(2, 2, figsize=(18, 10))
    for ax in axs.flat:
        ax.set(xlabel=xlabel)

    fig.suptitle(title)

    m_plt = axs[0, 0]
    cv_plt = axs[0, 1]
    chi_plt = axs[1, 0]
    mloc_plt = axs[1, 1]

    m_plt.plot(Ts, np.abs(Ms), marker='o', color='blue', linewidth=0.5)
    m_plt.set_ylabel("|m|")

    cv_plt.plot(Ts, Cvs, marker='x', color='red', linewidth=0.5)
    cv_plt.set_ylabel(r"$c_v$")

    chi_plt.plot(Ts, Chis, marker='+', color='green', linewidth=0.5)
    chi_plt.set_ylabel(r"$\chi$")

    M_loc_min = 1 / 2 # 1 / (dimension of M_z-space)
    mloc_plt.plot(Ts, np.exp(-MLocs), marker='+', color='blueviolet', linewidth=0.5)
    mloc_plt.axhline(y=M_loc_min, linestyle='dashed', color='gray', linewidth=1)
    mloc_plt.set_ylabel("magnetic localization")

    plt.savefig(fig_filename + ".png", dpi=300, format='png')
    plt.show()

def plot_fn(grid):
    from matplotlib import pyplot as plt

    ordering = np.empty_like(grid.next_idx)
    grid.ordered_indices(ordering)
    iFs = tuple(range(AdaptiveGrid.I0, grid.last_point_Fptr + 1))

    # plot
    Xs = [grid.xs[ordering[iF]] for iF in iFs]
    Ys = [grid.fs[ordering[iF]] for iF in iFs]
    _ymax = 1.05 * max(Ys)
    _ymin = min(Ys)
    for x in Xs:
        plt.vlines(x, _ymin, _ymax, linestyles='solid',
                   linewidth=0.5, color='gray')

    plt.plot(Xs, Ys, color='orangered', marker='',
             linestyle='solid',linewidth=1, markersize=1)
    #fig_metadata = '\n'.join(parameterStr(params))
    #plt.savefig(plotfile, dpi=300, format='svg', metadata={'Title': fig_metadata})
    plt.show()
#----------------------------------------

if __name__ == "__main__":
    # test_IsingMCMC()
    test_multi_IsingMCMC()
