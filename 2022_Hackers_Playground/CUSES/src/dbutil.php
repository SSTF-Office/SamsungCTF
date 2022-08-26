<?php
function find_from_db($username, $password) {
	if ($username === "guest" and $password === "guestpassword") {
		return 1;
	}

	return false;
}
