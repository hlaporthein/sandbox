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

	$objects = $s3Client->getIterator('ListObjects', array('Bucket' => $bucket));

	if (isset($_POST["upload"])) {

		$file = $_FILES["file"];
		$params = [
		    'Bucket' => $bucket,
		    'Key'    => $file["name"],
		    'Body'   => fopen($file["tmp_name"], 'r'),
		    'ContentType' => $file["type"]
		];
		$result = $s3Client->putObject($params);

		$uploadMessage = "File uploaded to bucket";
	}

	if (isset($_POST["retrieve-data"])) {

		$result = $s3Client->getObject(array(
		    'Bucket' => $bucket,
		    'Key'    => $_POST["file-selector"]
		));

		$retrieveData = "Content of '" . $_POST["file-selector"] . "': <br/>" . $result['Body'];

		$contentType = preg_match("/image/i", $result["ContentType"]);

		if ($contentType) {
			$retrieveData = "Content of '" . $_POST["file-selector"] . "': <br/> Image !";
		}

	}

	if (isset($_POST["delete-button"])) {

		$result = $s3Client->deleteObject(array(
			'Bucket' => $bucket,
			'Key'    => $_POST["delete-file-selector"]
		));

		$deleteMessage = "File '" . $_POST["delete-file-selector"] . "' successfully deleted";


	}




?>

<!DOCTYPE html>
<html>
	<head>
		<title>CRUD PHP</title>
		<style>
.box {
	padding: 10px;
	margin: 10px;
	width: 400px;
	background-color: hsla(0, 50%, 90%, 1);
}

form {
	margin-bottom: 20px;
}
		</style>
	</head>
	<body>
		<div class="box">
			<form method="post" enctype="multipart/form-data">
				Create file:<br/>
				<input type="file" name="file" /><br/>
				<button name="upload" type="submit">Upload to bucket</button>
			</form>
			<?php
				if (isset($uploadMessage) && !empty($uploadMessage)) {
					echo $uploadMessage;
				}
			?>
		</div>

		<div class="box">
			<form method="post">
				Retrieve file:<br/>
<?php
	$i = 0;
	foreach ($objects as $object) {
		$i++;
	}
	if ($i == 0) {
		echo "No file available<br/>";
	} else {
?>

				<select name="file-selector">
<?php
		foreach ($objects as $object) {
			echo '<option value="' . $object["Key"] . '">' . $object["Key"] . '</option>';
		}
?>
				</select><br/>
<?php
	}
?>
				<button name="retrieve-data">Retrieve file</button>
			</form>
			<?php
				if (isset($retrieveData) && !empty($retrieveData)) {
					echo $retrieveData;
				}
			?>
		</div>

		<div class="box">
			<form method="post">
				Delete file:<br/>
<?php
	if ($i == 0) {
		echo "No file available<br/>";
	} else {
?>
				<select name="delete-file-selector">
<?php
		foreach ($objects as $object) {
			echo '<option value="' . $object["Key"] . '">' . $object["Key"] . '</option>';
		}
?>
				</select><br/>
<?php
	}
?>
				<button name="delete-button" type="submit">Delete from bucket</button>
			</form>
			<?php
				if (isset($deleteMessage) && !empty($deleteMessage)) {
					echo $deleteMessage;
				}
			?>
		</div>

	</body>
</html>