################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.c" 

COMPILED_SRCS += \
"Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.src" 

C_DEPS += \
"./Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.d" 

OBJS += \
"Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.src":"../Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.c" "Libraries/iLLD/TC37A/Tricore/I2c/I2c/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fZ:/home/mr_monopoly/Alberto/programmazione/Cproject/raceup/ControlUnitLogicOperator/instances/aurix_375_lte/TriCore Release (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.o":"Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.src" "Libraries/iLLD/TC37A/Tricore/I2c/I2c/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-I2c-2f-I2c

clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-I2c-2f-I2c:
	-$(RM) ./Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.d ./Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.o ./Libraries/iLLD/TC37A/Tricore/I2c/I2c/IfxI2c_I2c.src

.PHONY: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-I2c-2f-I2c
