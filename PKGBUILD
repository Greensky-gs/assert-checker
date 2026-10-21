pkgname=assert-checker
_name=assert-checker
pkgver=1.0.0
pkgrel=1
pkgdesc="A simple logical assertions checker"
arch=("x86_64" "aarch75")
url="https://git.greensky.tf/Greensky/assert-checker"
license=("unknown")
depends=()
makedepends=('git' 'gcc' 'make')
optdepends=()
source=("git+${url}#commit=914a541159966541b484ee68971eb2f0b2195be6")
options=("!debug")
sha256sums=("SKIP")

build() {
  cd "$srcdir/$_name"

  make
}
package() {
  cd "$srcdir/$_name"

  # Binary
  install -Dm755 main.exe "$pkgdir/usr/bin/assert-checker"
}
