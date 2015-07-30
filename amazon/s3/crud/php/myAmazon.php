<?php
	require '/lib/aws-autoloader.php';

	use Aws\S3\S3Client;
	require 'credentials.php';

	// Instantiate the S3 client with your AWS credentials
	$s3Client = S3Client::factory(array(
	'version'     => 'latest',
    'region'      => 'eu-central-1',
	'credentials' => array(
		'key'    => $credentials["key"],
		'secret' => $credentials["secret"]
		)
	));

	$bucket = "jlg-test-bucket";



	if (isset($_POST["upload"])) {

		$file = $_FILES["file"];
		$params = [
		    'Bucket' => $bucket,
		    'Key'    => $file["name"],
		    'Body'   => fopen($file["tmp_name"], 'r'),
		    'ContentType' => $file["type"]
		];
		$result = $s3Client->putObject($params);

		echo json_encode(["status"	=> "ok"]);
	}

	if (isset($_GET["retrieve"])) {

		$objects = $s3Client->getIterator('ListObjects', array('Bucket' => $bucket));

		$result = [];

		foreach ($objects as $object) {
			$result[] = [
				"name" => $object["Key"]
			];
		}

		echo json_encode($result);

	}

	if (isset($_GET["retrieve-data"])) {

		$result = $s3Client->getObject(array(
		    'Bucket' => $bucket,
		    'Key'    => $_GET["retrieve-data"]
		));

		header('Content-Type: ' . $result["ContentType"]);

		echo $result['Body'];

	}

	if (isset($_GET["delete-data"])) {

		$result = $s3Client->deleteObject(array(
			'Bucket' => $bucket,
			'Key'    => $_GET["delete-data"]
		));

		echo "File '" . $_GET["delete-data"] . "' successfully deleted";


	}




?>

