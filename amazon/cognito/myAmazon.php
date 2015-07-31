<?php
	require 'credentials.php';
	require '/lib/aws-autoloader.php';

	use Aws\CognitoIdentity\CognitoIdentityClient;

	$client = CognitoIdentityClient::factory(array(
		'version'     => 'latest',
	    'region'      => 'eu-west-1',
	    'credentials' => array(
			'key'    => $credentials["key"],
			'secret' => $credentials["secret"]
		)
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

	if (isset($_GET["login"])) {

		$login = $_GET["login"];
		$password = $_GET["password"];

		if (!($login == 'maou' && $password == 'maou')) {
			echo json_encode(["status"	=> "ko", "error"	=> "bad authentication"]);
			return;
		}

		$result = $client->getOpenIdTokenForDeveloperIdentity(array(
			'IdentityPoolId' => 'eu-west-1:f01a8597-951e-4907-aeab-0ee690f50e40',
			'Logins' => array(
				'login.jlg-consulting.com' => 'maou'
			),
			'TokenDuration' => 15*60
		));

		myLog('result = ' . grab_dump($result));


		echo json_encode([
			"status" => "ok",
			"data" => [
				"IdentityId"	=> $result["IdentityId"],
				"Token"	=> $result["Token"]
			]]);
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

