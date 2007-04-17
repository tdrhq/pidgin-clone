<xsl:stylesheet version = '1.0'
 xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:output method="html"/>
<xsl:template match="conversation">
     	<h3 align="center"> ---- New Conversation @ 
	<xsl:value-of select="@date" />
	<xsl:text> </xsl:text>
     	<xsl:value-of select="@time" />
 	<xsl:text> ----</xsl:text>
	</h3><br/>
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="message[@from]">
	<xsl:choose>
	<xsl:when test="@type = 'sent'">
		<font color="#16569E"><font size="2">
		(<xsl:value-of select="@time" />) </font>
		<b><xsl:value-of select="@from" />: </b></font>
	</xsl:when>
	<xsl:when test="@type = 'received'">
                 <font color="#A82F2F"><font size="2">
		 (<xsl:value-of select="@time" />) </font>
                 <b><xsl:value-of select="@from" />: </b></font>
	</xsl:when>
	</xsl:choose>
	<xsl:apply-templates/><br />
</xsl:template>
	
<xsl:template match="message">
	<font color="#000000"><font size="2">
	(<xsl:value-of select="@time" />) </font>
	<b><xsl:apply-templates /></b></font>
</xsl:template>
</xsl:stylesheet>
