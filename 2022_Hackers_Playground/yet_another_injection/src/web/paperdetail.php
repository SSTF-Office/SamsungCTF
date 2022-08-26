<?php
	session_start();

	require_once 'library.php';
	$papers = loadPapers('papers.xml');

	header("Content-Type:application/json");

	if (!isset($_SESSION["username"])) {
		echo json_encode(['status' => 'Error', 'msg' => 'Forbidden']);
	} else if(!isset($_GET['idx'])) {
		echo json_encode(['status' => 'Error', 'msg' => 'Invalid Request']);
	} else {
		$idx = $_GET['idx'];
		$paper = getDetail($papers, $idx);
		echo json_encode($paper);
	}
?>
