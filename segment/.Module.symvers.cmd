cmd_/home/pi/segment/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/segment/modules.order | scripts/mod/modpost -m -a    -o /home/pi/segment/Module.symvers -e -i Module.symvers -T - 
