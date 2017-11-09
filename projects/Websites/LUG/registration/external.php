<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table2">
	<tr>
		<td>
		<h3><a href=".">Home</a> : Registration(user)</h3>
		</td>
	</tr>
</table>

<h1>Welcome to LUG NIT Durgapur Registration. Its <b><i>FREE</i></b></h1>
<h2>If u are a student of NIT Durgapur, please <a onclick="showpage('registration/internal.php')">register as a Member of LUG NITDgp.</a></h2>


<form method="post" action="registration/register_external.php" onsubmit="return validate('username','pass','from','linux')">
<table border="0" cellpadding="0" id="table1" cellspacing="7">
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Username :</td>
		<td><input type="text" name="username" id="username">* will appear across ur uploads</td>
	</tr>
	<tr>
		<td align="right"></td>
		<td><input type="button" value="Check Availability" id="check"  onClick="check_availability()"></td>
	</tr>
	<tr>
		<td align="right">Password :</td>
		<td><input type="password" name="password" id="pass">*</td>
	</tr>
	<tr>
		<td align="right">Re-type Password :</td>
		<td><input type="password" name="retype" id="repass"></td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
		<tr>
		<td align="right"><h3>Personal Information</h3></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Name :</td>
		<td><input type="text" name="name"></td>
	</tr>
	<tr>
		<td align="right"><i>(place)</i> From :</td>
		<td><input type="text" name="from" id="from">*</td>
	</tr>
	<tr>
		<td align="right">Date of Birth :</td>
		<td><select name="dd" size="1">
			<option selected>day</option>
			<option>01</option>
			<option>02</option>
			<option>03</option>
			<option>04</option>
			<option>05</option>
			<option>06</option>
			<option>07</option>
			<option>08</option>
			<option>09</option>
			<option>10</option>
			<option>11</option>
			<option>12</option>
			<option>13</option>
			<option>14</option>
			<option>15</option>
			<option>16</option>
			<option>17</option>
			<option>18</option>
			<option>19</option>
			<option>20</option>
			<option>21</option>
			<option>22</option>
			<option>23</option>
			<option>24</option>
			<option>25</option>
			<option>26</option>
			<option>27</option>
			<option>28</option>
			<option>29</option>
			<option>30</option>
			<option>31</option>			
			</select>
			<select name="mm" size="1">
			<option selected>month</option>
			<option>January</option>
			<option>February</option>
			<option>March</option>
			<option>April</option>
			<option>May</option>
			<option>June</option>
			<option>July</option>
			<option>August</option>
			<option>September</option>
			<option>October</option>
			<option>November</option>
			<option>December</option>						
			</select>
			<select name="yyyy" size="1">
			<option selected>year</option>
			<script>
			for(i=2000;i>1946;i--)
				document.write('<option>'+i+'</option>');
			</script>			
			</select>
		</td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right"><h3>Contact Details</h3></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">E-mail ID :</td>
		<td><input type="text" name="email"></td>
	</tr>
	<tr>
		<td align="right">Phone no :</td>
		<td><input type="text" name="phone"></td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right"><h3>Professional Information</h3></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Institute :</td>
		<td><input type="text" name="inst"></td>
	</tr>
	<tr>
		<td align="right">Specialization :</td>
		<td><input type="text" name="spec"></td>
	</tr>
	<tr>
		<td align="right">Others (occupation / year, etc etc) :</td>
		<td><input type="text" name="other"></td>
	</tr>
	<tr>
		<td align="right">About Linux :</td>
		<td>
		<select name="linux" size="1" id="linux">
			<option selected value="">Choose</option>
			<option value="10">I devolop Linux</option>
			<option value="9">I am a Linux GURU</option>
			<option value="8">I am a Linux Professional</option>
			<option value="7">I am always on Linux</option>
			<option value="6">I love Linux</option>
			<option value="5">I have worked on Linux</option>
			<option value="4">I know a little Linux</option>
			<option value="3">I have just started Linux</option>
			<option value="2">Ya, I have seen Linux</option>
			<option value="1">I have heard abour Linux</option>
			<option value="0">Linux!! Whos that!!</option>						
		</select>
		*</td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td><input type="submit" value="Register Me"></td>
	</tr>
	
</table>
</form>
</center>