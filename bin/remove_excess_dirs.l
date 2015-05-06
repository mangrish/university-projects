
%s WORKSPACES

%%

^[^\n:]*":\n"/"\n./"			;
^"./workspaces/"[^\n]*			{BEGIN WORKSPACES;}
^"./www_public/"				{printf("%s", yytext);
								BEGIN 0;}
<WORKSPACES>.					;

^"./www_public/web:"			;

%%

