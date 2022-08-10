
# Building

*How to setup & build this project.*

<br>

## Requirements

-   **[DevKitPro]**

-   **[GCC]**

<br>

## Preparation

*Windows only.*

1.  Launch **msys2** provided by **devkitPro**

2.  Run the command 

    ```Shell
    pacman -S gcc
    ```

<br>

## Instructions

<br>

1.  Obtain a clean **DOL** of `Super Mario Sunshine JP 1.0`

    <br>

2.  Place it in the root folder with `baserom.dol` as its name.

    <br>

3.  Download <kbd>**[GC_WII_COMPILERS.zip][GC]**</kbd>

    <br>

4.  Extract it's content to `/tools/mwcc_compiler/`

    <br>

5.  Run the **Make** command :

    ```sh
    make
    ```

<br>


<!----------------------------------------------------------------------------->

[DevKitPro]: https://devkitpro.org/wiki/Getting_Started
[GCC]: https://gcc.gnu.org/
[GC]: https://cdn.discordapp.com/attachments/727918646525165659/917185027656286218/GC_WII_COMPILERS.zip
