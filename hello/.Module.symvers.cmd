cmd_/home/pi/hello/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/hello/modules.order | scripts/mod/modpost -m -a    -o /home/pi/hello/Module.symvers -e -i Module.symvers -T - 
