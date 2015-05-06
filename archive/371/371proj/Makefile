#
# Simple makefile to build or run the Enroller
#
# Tim Dwyer - August 2000
#
OBJ_UTILS = utils/HelpFrame.class \
            utils/ExampleFileFilter.class
#
# If you add any classes put them here:
#
OBJ_ENROLLER = enroller/Enroller.class \
               enroller/EnrolmentFrame.class \
               enroller/Enrolment.class

OBJ_JAVADOC = HelpFiles/overview-tree.html \
              HelpFiles/index-all.html \
              HelpFiles/deprecated-list.html \
              HelpFiles/allclasses-frame.html \
              HelpFiles/index.html \
              HelpFiles/packages.html \
              HelpFiles/overview-summary.html \
              HelpFiles/overview-frame.html \
              HelpFiles/enroller/package-summary.html \
              HelpFiles/enroller/package-tree.html \
              HelpFiles/enroller/package-frame.html \
              HelpFiles/utils/package-summary.html \
              HelpFiles/utils/package-tree.html \
              HelpFiles/utils/package-frame.html \
              HelpFiles/enroller/Enroller.html \
              HelpFiles/enroller/Enrolment.html \
              HelpFiles/enroller/Enrolment.Day.html \
              HelpFiles/enroller/Enrolment.Day.Time.html \
              HelpFiles/enroller/Enrolment.Day.Time.EnrolledList.html \
              HelpFiles/enroller/Enrolment.Day.TimeList.html \
              HelpFiles/enroller/Enrolment.DayList.html \
              HelpFiles/enroller/EnrolmentFrame.html \
              HelpFiles/utils/ExampleFileFilter.html \
              HelpFiles/utils/HelpFrame.html \
              HelpFiles/serialized-form.html \
              HelpFiles/package-list \
              HelpFiles/help-doc.html \
              HelpFiles/stylesheet.css

CLASSPATH = .:jaxp.jar:parser.jar:$(JAVA_HOME)/lib/classes.zip
# replace CLASSPATH with the following if your using this Makefile
# under cygwin bash
#CLASSPATH = .;jaxp.jar;parser.jar;$(JAVA_HOME)\\lib\\classes.zip
JAVADOCDIR = HelpFiles

default: ${OBJ_UTILS} ${OBJ_ENROLLER} 
default: javadoc ${OBJ_UTILS} ${OBJ_ENROLLER} ${OBJ_JAVADOC}
	@echo "make ... to build Enroller"
	@echo "make run ... to execute with the current user as argument"
	@echo "make javadoc ... extract javadoc comments and put the result into HelpFiles directory"
	@echo "make clean ... remove class files"

%.class : %.java
	javac -O -classpath ${CLASSPATH} $<	

javadoc:
	javadoc -d ${JAVADOCDIR} -overview overview.html utils enroller


#
#
run:	${OBJ_UTILS} ${OBJ_ENROLLER}
	@java -classpath .:jaxp.jar:parser.jar:$(JAVA_HOME)/lib/classes.zip \
		enroller/Enroller ${USER}

clean:
	-rm -f ${OBJ_UTILS} ${OBJ_ENROLLER} ${OBJ_JAVADOC} \
        enroller/*.class utils/*.class
