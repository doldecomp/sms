
# Super Mario Sunshine [![Discord Badge]][Discord]

*A* ***WIP*** *decompilation of the* ***0th*** *revision of the* ***JP*** *version.*

<br>

---

## DOL

*The* ***DOL*** *built with this repository.*

**File** : `sms_jp_r0.dol` <br>
**SHA1** : `9f5a8caf56f5356aeac9d3ed28bf8de976a03625`

---

<br>

## Building

### Requirements

- **[DevKitPro]**

- **[GCC]**

    ```sh
    pacman -S gcc
    ```

<br>

### Preparation

*Windows only.*

-   Launch msys2(Provided by devkitPro) and run the command `pacman -S gcc`.

<br>

### Instructions

1. Obtain a clean **DOL** of `Super Mario Sunshine JP 1.0`.

2. Place it in the root folder with `baserom.dol` as its name.

3. Download <kbd>**[GC_WII_COMPILERS.zip][GC]**</kbd>

4. Extract it's content to `/tools/mwcc_compiler/`.

5. Run the **Make** command :

    ```sh
    make
    ```

<br>

---

<br>

## Contributions

*`Contributions` / `Pull Requests` are welcome !*



<!----------------------------------------------------------------------------->

[DevKitPro]: https://devkitpro.org/wiki/Getting_Started
[GCC]: https://gcc.gnu.org/
[GC]: https://cdn.discordapp.com/attachments/727918646525165659/917185027656286218/GC_WII_COMPILERS.zip

[Discord Badge]: https://img.shields.io/badge/Discord-7289DA?style=for-the-badge&logo=discord&logoColor=white
[Discord]: https://discord.gg/hKx3FJJgrV
