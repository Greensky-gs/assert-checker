# Assert checker

Ceci est un véricateur d'assertions logiques simple écrit en C

## Installation

Suivez les étapes d'installation selon votre plateforme

- [Linux](#linux)
- [Windows](#windows)

MacOS n'est pas listé, c'est parce que je n'ai jamais essayé. Tout changement est le bienvenue, si vous avez essayé l'installation Mac

### Linux

1. Installer les dépendences suivantes : `git`, `gcc` et `make`. Par exemple, pour les distributions basées sur Debian : `sudo apt install git gcc make`
2. Copier le code source (avec git) : `git clone https://git.greensky.tf/Greensky/assert-checker && cd assert-checker`
3. Créer l'exécutable : `make`
4. Vous pouvez maintenant utiliser le fichier `main.exe`, avec les [instructions d'utilisation](#installation)

> Utiliser les commandes ci-dessus dans un terminal

### Windows

1. Installer les dépendences : `git`, `gcc` et `make` via les sites d'installation, ou avec winget :
```
winget install -e --id Arm.GnuArmEmbeddedToolchain
winget install -e --id GnuWin32.Make
winget install -e --id Git.Git
```
2. Copier le code source (avec git) : `git clone https://git.greensky.tf/Greensky/assert-checker && cd assert-checker`
3. Créer l'exécutable : `make`
4. Vous pouvez maintenant utiliser le fichier `main.exe`, avec les [instructions d'utilisation](#installation)

> Utiliser les commandes ci-dessus dans l'invite de commande (windows + R, puis taper `cmd` puis entrée)

## Utilisation

Une fois que le fichier `main.exe` est crée, vous pouvez l'utiliser en exécutant le script. Il vous demandera alors une expression. Une expression est composée de :

- Variables, représentée par une lettre par variable
- Opérateurs
- Parenthèses, qui créent des groupes

### Liste d'opérateurs

| Opérateur | Symbole | Syntaxe |
|:---------:|:-------:|:-------:|
| **Et** logique | `&` | `X&Y` |
| **Ou** logique | `\|` | `X|Y` |
| **Non** logique | `!` | `!X` |
| **Implication** | `-` | `X-Y` |
| Groupe d'assertion | `(` et `)` | `(X)` |

> Dans les descriptions de syntaxes ci-dessus, X et Y sont des assertions valides, c'est-à-dire soit : une variable, soit une ou deux assertions assemblées par des opérateurs

### Exemples

Tout les exemples ci-dessous sont des expressions valides que le programme peut lire (une par ligne)

```
a
a&!c
(a&c)-d
(a|(b&d))|c-d
(a|b)-(c&(d|v)|a)
(((!p&s)|(g|!s))|(!t&!g))&((s|!g)&(r|s))
```

## Complexité

Si **n** est le nombre de **variables différentes**, le programme a une complexité de `O(2^n)` en temps

Puisque le programme est écrit en C natif, il va très vite de toutes façons

## Contributeurs

- [ @Greensky-gs ](https://git.greensky.tf/Greensky)
