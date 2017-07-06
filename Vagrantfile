Vagrant.configure(2)  do |config|
  config.vm.box = "ubuntu/trusty64"
  config.vm.hostname = "sparkdev"
#  config.vm.network :forwarded_port, guest: 9000, host: 9000
  config.vm.network "private_network", ip: "192.168.33.253"
  config.vm.provider "virtualbox" do |v|
    v.memory = 512
  end
  config.vm.provision :docker  
  config.vm.provision :shell, path: 'bootstrap.sh', keep_color: true, privileged: true

end
