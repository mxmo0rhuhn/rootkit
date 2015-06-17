# -*- mode: ruby -*-
# vi: set ft=ruby :

$vb_gui = false
$vb_memory = 1024
$vb_cpus = 1

Vagrant.configure(2) do |config|
  config.vm.box = "Gentoo_amd64_minimal"
  config.vm.box_url = "https://dl.dropboxusercontent.com/s/mfurnvstqoj8w47/gentoo-20131024-amd64-minimal.box"
  config.vm.box_check_update = false

  config.vm.provider "virtualbox" do |vb|
    vb.gui = $vb_gui
    vb.memory = $vb_memory
    vb.cpus = $vb_cpus
  end

  ["3.10.77", "4.0.5"].each do |j|
    config.vm.define vm_name = "kernel-%s" % j do |kernel|
      kernel.vm.hostname = vm_name
      kernel.vm.synced_folder "./src", "/vagrant_src"

      kernel.vm.provision "shell", inline: <<-SHELL
        sudo sh -c 'echo "sys-kernel/gentoo-sources ~amd64" >> /etc/portage/package.accept_keywords/kernel'
        sudo emerge -q --sync
      SHELL
      kernel.vm.provision :shell, :inline => "sudo emerge -q =sys-kernel/gentoo-sources-%s" % j, :privileged => true
      kernel.vm.provision "shell", inline: <<-SHELL
        sudo genkernel --install --symlink --oldconfig --bootloader=grub all
        sudo grub2-mkconfig -o /boot/grub/grub.cfg
        sudo grub2-install --no-floppy /dev/sda
        sudo reboot
      SHELL
    end
  end
end
