console.log('coucou');

var Download = require('download');
var AdmZip = require('adm-zip');
var tmp = './temp';
require('shelljs/global');

var d = new Download({mode: '755'})
	.get('https://github.com/aws/aws-sdk-php/releases/download/2.8.16/aws.zip')
	.dest(tmp)
	.run(function() {
		console.log('end');
		var zip = new AdmZip(tmp + '/aws.zip');
		zip.extractAllTo('./lib/');
		rm('-rf', tmp);
		console.log('Successfully installed');
	});
