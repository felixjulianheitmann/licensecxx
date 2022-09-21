.. _identifiers_doc:

Identifiers
===========

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Licensecxx allows to gather information about the system currently running the software. To use those features, link against the ``lcxx::identifiers`` target in your CMake configuration.

This will provide the following headers:
- ``lcxx/identifiers/hardware.hpp``
- ``lcxx/identifiers/os.hpp``

There two primary functions are given (arguments omitted):
- ``lcxx::identifiers::<header-name>()`` (e.g. ``lcxx::identifiers::hardware()``)
- ``lcxx::identifiers::verify()``

The type of information that will be collected depends on the chosen strategy. Sticking to the ``hardware`` example, the enum ``hw_ident_strat`` defines which information will be incorporated into the hardware identifier.

Passing the value ``hw_ident_strat::all`` will try to collect as much info about the machine and generate a single ``identifier`` object from that. This object contains a string containing all collect info and a hash-digest of that string.

This hash (or the respective string) can be included as a value into the license.

The ``lcxx::identifiers::verify`` function takes a ``hw_ident_strat`` and a hash value and will check whether those match, given the current underlying hardware.

Multiple identification strategies can be combined by using the ``operator|``. Using ``hw_ident_strat::cpu_n_cores | hw_ident_strat::cpu_model_name`` will only collect the number of 
available CPU cores and the CPU model name.

The following code snipped would take a license object and push in the hardware identifier consisting of no. of CPU cores and its model name.

.. code-block:: c++

    #include <lcxx/lcxx.hpp>
    #include <lcxx/identifiers/hardware.hpp>

    int main()
    {
        using namespace lcxx;
        using hw_strat = identifiers::hw_ident_strat;

        license lic;
        auto [hash, text] = identifiers::hardware( hw_strat::cpu_n_cores | hw_strat::cpu_model_name );
        lic.push_content( "hardware", hash );

        // ... rest of the software
    }


Hardware
--------

The hardware header (``lcxx/identifier/hardware.hpp``) allows to collect information about the systems hardware, if the current user has the privileges to access that information.

The available strategies are listed by the ``hw_ident_strat`` enum and have the following meaning

================= ==================================================================================================================
Enumerator        Meaning
================= ==================================================================================================================
all               All available information is gathered. Any other additionally set strategy is ignored.
cpu               All information regarding the builtin CPU is gathered. Any other additionally, set CPU strategy is ignored.
cpu_n_cores       Try to collect the number physical CPU cores.
cpu_n_threads     Try to detect the number of available hardware threads.
cpu_max_frequency Try to discover the maximum frequency in Hz of the builtin CPU.
cpu_vendor        Try to discover the vendor name of the CPU.
cpu_model_name    Try to discover the CPU's model name.
================= ==================================================================================================================

Note, that multi-CPU setups are not supported. Such a setup may yield unexpected values for CPU related info.

Operating System
----------------

The OS header (``lcxx/identifier/os.hpp``) allows to collect information about the operating system and the current user, if the current user has the privileges to access that information.

The available strategies are listed by the ``os_ident_strat`` enum and have the following meaning

================= ==================================================================================================================
Enumerator        Meaning
================= ==================================================================================================================
all               All available information is gathered. Any other additionally set strategy is ignored.
os                All available information regarding the OS is gathered. Any other additionally set OS strategies are ignored.
user              All available information regarding the user is gathered. Any other additionally set user strategies are ignored.
os_name           Try to discover the OS name (e.g. ``Linux``)
os_architecture   Try to discover the OS architecture
os_pc_name        Try to discover the OS PC name (or hostname)
user_name         Try to discover the currently logged in user's name
user_uid          Try to discover the currently logged in user's id 
user_groups       Try to discover the groups the currently logged in user belongs to 
user_gids         Try to discover the group ids the currently logged in user belongs to 
================= ==================================================================================================================
