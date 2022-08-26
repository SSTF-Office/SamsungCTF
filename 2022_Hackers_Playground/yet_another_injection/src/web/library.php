<?php
	$_library_file_name = '/db/papers.xml';

	function loadPapers() {
		global $_library_file_name;
		$xml = new DOMDocument;
		$xml->load($_library_file_name);
		return $xml;
	}

	function paperList(DomDocument $papers) {
		$xpath = new DOMXPath($papers);
		$query = '/Papers/Paper[@published="yes"]';
		$paper_list = $xpath->query($query);
		return $paper_list;
	}

	function getFirstChildText(DOMNode $node, string $tag) {
		return $node->getElementsByTagName($tag)->item(0)->nodeValue;
	}

	function getDetail(DomDocument $papers, string $idx) {
		$xpath = new DOMXPath($papers);
		$query = "//Paper[Idx/text()='".$idx."' and @published='yes']";
		$paper_list = $xpath->query($query);
		
		if ($paper_list == false) {
			return ['status' => 'Error', 'msg' => 'Invalid XPATH expression'];
		}
		if ($paper_list->count() == 0) {
			return ['status' => 'Error', 'msg' => 'No such entity'];
		}

		$paper = $paper_list->item(0);
		return [
			'status' => 'Success', 
			'Title' => getFirstChildText($paper, 'Title'), 
			'Author' => getFirstChildText($paper, 'Author'), 
			'Conference' => getFirstChildText($paper, 'Conference'), 
			'Year' => getFirstChildText($paper, 'Year'), 
			'Abstract' => getFirstChildText($paper, 'Abstract')
		];
	}
?>
