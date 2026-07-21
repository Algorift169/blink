#!/usr/bin/env bash
set -euo pipefail

if [[ $EUID -ne 0 ]]; then
  echo "This script installs system packages. Re-run it with sudo." >&2
  exit 1
fi

if command -v apt-get >/dev/null 2>&1; then
  apt-get update
  apt-get install -y build-essential pkg-config libgtk-3-dev libvte-2.91-dev
elif command -v dnf >/dev/null 2>&1; then
  dnf install -y gcc-c++ make pkg-config gtk3-devel vte291-devel
elif command -v pacman >/dev/null 2>&1; then
  pacman -Syu --noconfirm base-devel pkgconf gtk3 vte3
elif command -v zypper >/dev/null 2>&1; then
  zypper install -y gcc-c++ make pkg-config gtk3-devel vte291-devel
else
  echo "Unsupported package manager. Install the dependencies manually." >&2
  exit 1
fi
