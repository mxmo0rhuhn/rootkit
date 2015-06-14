#!/bin/bash

vagrant box add Gentoo_amd64_minimal https://dl.dropboxusercontent.com/s/mfurnvstqoj8w47/gentoo-20131024-amd64-minimal.box
vagrant init Gentoo_amd64_minimal
vagrant up
