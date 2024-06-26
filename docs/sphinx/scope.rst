Scope of Guildander
===================

Guilander gloas
---------------

- C++ impelementation of Wayland protocol for clients.
- Be agnostic of:
    - async model
- Give example usecase:
    - print all globals
    - write ascii
    - draw ascii box
    - ascii grid

Guilander non-goals
-------------------

- Event loop implementation.
- Raii abstarction for Wayland objects.
- Anything that needs async library.
- Impelements any async library funcionalities in itself.
    - Slippery slope to add asyc features in sstd or similar.
- Abstracts syscalls from different operating systems.

.. note::

    Previously there was recommendation that:

    ::

        Only abstract just enough
        that spesific usecase is behind raii. (*)

    but with more though I think this is anti-pattern(?),
    as it leads to clunky abstractions.
    See operating system requirements for more discussion.

Operating system requirements
-----------------------------

Guilander is tested only for Linux.

Syscall abstraction requirements
--------------------------------

Guilanded syscall abstractions have to be comprehensive enough,
that there is confidence to use them in testing the Waylanad IPC mechanism.

This include following usecases:

- opening and closing files
- creating and connecting to unix/local stream socket
- allocating and freeing of all used resources exposed via raii
- reading and writing operations for generic file descriptors based on std::span

.. note::

    However,
    as semantics of reading and writing could depend on the spesific file descriptor type,
    these generic operations are customizeable via information encoded in to the C++ type of
    a raii object holding a file descriptor.

    This information does not have defaults, but it should be oneliner to opt into a semantic.
