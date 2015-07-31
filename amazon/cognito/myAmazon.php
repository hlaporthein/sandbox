<?php
	require 'credentials.php';
	require '/lib/aws-autoloader.php';

	use Aws\S3\S3Client;
	use Aws\CognitoIdentity\CognitoIdentityClient;

	// Instantiate the S3 client with your AWS credentials
	$s3Client = S3Client::factory(array(
		'version'     => 'latest',
	    'region'      => 'eu-central-1',
		'credentials' => array(
			'key'    => $credentials["key"],
			'secret' => $credentials["secret"]
		)
	));

	$cognitoClient = CognitoIdentityClient::factory(array(
		'version'     => 'latest',
	    'region'      => 'eu-west-1'
	));

	$result = $client->getOpenIdTokenForDeveloperIdentity(array(
		'IdentityPoolId' => 'string',
		'IdentityId' => 'string',
		'Logins' => array(
			'IdentityProviderName' => 'string'
		),
		'TokenDuration' => integer,
	));

	$bucket = "jlg-test-bucket";

	$params = json_decode(file_get_contents('php://input'),true);
	$_POST = $params;

	function grab_dump($var) {
	    ob_start();
	    print_r($var);
	    return ob_get_clean();
	}

	function myLog($string) {
		file_put_contents('php.log', $string . "\n", FILE_APPEND | LOCK_EX);
	}

	//myLog('param = ' . grab_dump($params));

	if (isset($params["login"])) {

		$login = $_POST["login"];
		$password = $_POST["password"];

		if (!($login == 'juan' && $password == 'juan')) {
			echo json_encode(["status"	=> "ko", "error"	=> "bad authentication"]);
			return;
		}


		echo json_encode(["status"	=> "ok"]);
		return;
	}

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

