.PHONY: clean All Project_Title Project_Build Project_PostBuild

All: Project_Title Project_Build Project_PostBuild

Project_Title:
	@echo "----------Building project:[ gpio - BuildSet ]----------"

Project_Build:
	@make -r -f gpio.mk -j 32 -C  gpio/projects/examples/driver/gpio/CDK 

Project_PostBuild:
	@echo Executing Post Build commands ...
	@cd gpio/projects/examples/driver/gpio/CDK && export CDKPath="C:/C-Sky/CDK" CDK_VERSION="V2.24.2" ProjectName="gpio" ProjectPath="C:/projects/dsPIC/aim5101/gpio/projects/examples/driver/gpio/CDK/" && "C:/projects/dsPIC/aim5101/gpio/projects/examples/driver/gpio/CDK/../../../../../utilities//aft_build.sh"
	@echo Done


clean:
	@echo "----------Cleaning project:[ gpio - BuildSet ]----------"

