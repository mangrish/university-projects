set CLASSPATH=.;jaxp.jar;parser.jar;$(JAVA_HOME)\lib\classes.zip
java -classpath %CLASSPATH% enroller/Enroller %USERNAME%

