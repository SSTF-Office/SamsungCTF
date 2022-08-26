<?php
	session_start();

	if (!isset($_SESSION["username"])) {
		header("Location: login.php");
	}

	require_once 'library.php';
	$papers = loadPapers('papers.xml');
?>


<!DOCTYPE html>
<html lang="en">
<head>
	<title>Yet Another Injection</title>
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css" integrity="sha384-zCbKRCUGaJDkqS1kPbPd7TveP5iyJE0EjAuZQTgFLD2ylzuqKfdKlfG/eSrtxUkn" crossorigin="anonymous">
	<script src="js/jquery-3.5.1.min.js"></script>
	<script src="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-fQybjgWLrvvRgtW6bFlB7jaZrFsaBXjsOMm/tB9LTS58ONXgqbR9W8oWht/amnpF" crossorigin="anonymous"></script>
	<link rel="stylesheet" href="css/style.css">
</head>
<body>
	<script>
		function show_detail(idx) {
			$.getJSON("/paperdetail.php?idx="+idx, 
				function(data) {
					$('#title').text(data.Title);
					$('#author').text(data.Author);
					$('#conference').text(data.Conference);
					$('#year').text(data.Year);
					$('#abstract').text(data.Abstract);
					$("#detailmodal").modal('show');
				}
			);
		}
	</script>

	<div class="container">
		<div class="w-100 shadow-lg p-3 my-5 bg-white rounded">
			<h1>Research Paper Repository</h1>
			<div class="text-right">
				<?php
					echo "<p><b>".$_SESSION["username"]."</b>&nbsp";
				?>
				(<a href='logout.php'>Logout</a>)</p>
			</div>

			<!-- Modal -->
			<div class="modal fade bd-example-modal-lg" tabindex="-1" role="dialog" aria-labelledby="myLargeModalLabel" aria-hidden="true" id="detailmodal">
				<div class="modal-dialog modal-lg modal-dialog-centered" role="document">
					<div class="modal-content">
						<div class="modal-header">
							<h4 class="modal-title px-5" id="title">Paper Title</h4>
						</div>
						<div class="modal-body">
							<div class="text-center">
								<table border="1" width="90%">
									<tr><td width="20%"><b>1st Author</b></td><td colspan="3" id="author"></td></tr>
									<tr><td width="20%"><b>Conference</b></td><td width="50%" id="conference"></td><td width="20%"><b>Year</b></td><td id="year"></td></tr>
								</table>
							</div>
							<div class="px-5 pt-3">
								<h3>Abstract</h3>
								<p id="abstract"></p>
							</div>
						</div>
						<div class="modal-footer">
							<button type="button" class="btn btn-primary" data-dismiss="modal">Close</button>
						</div>
					</div>
				</div>
			</div>

			<table class="table">
				<thead class="thead-dark">
					<tr>
						<th scope="col">#</th>
						<th scope="col">Year</th>
						<th scope="col">Title</th>
						<th scope="col">1st author</th>
					</tr>
				</thead>
				<tbody>
					<?php
						foreach (paperList($papers) as $paper) {
							$row = array_map(
										function($tag) use ($paper) {return getFirstChildText($paper, $tag);},
										Array('Idx', 'Year', 'Title', 'Author')
									);
							echo "<tr onclick='show_detail({$row[0]});'><th scope='row'>{$row[0]}</th><td>{$row[1]}</td><td>{$row[2]}</td><td>{$row[3]}</td></tr>";
						}
					?>
				</tbody>
			</table>
		</div>
	</div>
</body>
</html>

