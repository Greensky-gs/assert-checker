pkgname=assert-checker
_name=assert-checker
pkgver=1.1.0
pkgrel=1
pkgdesc="A simple logical assertions checker"
arch=("x86_64" "aarch64")
url="https://git.greensky.tf/Greensky/assert-checker"
license=("unknown")
depends=()
makedepends=('git' 'gcc' 'make')
optdepends=()
source=("git+${url}#commit=86233418cd3bbadee792d1066812f066404224aa")
options=("!debug")
sha256sums=("SKIP")

build() {
  cd "$srcdir/$_name"

  make cleanbuild
}
package() {
  cd "$srcdir/$_name"

  # Binary
  install -Dm755 main.uwu "$pkgdir/usr/bin/assert-checker"
}
