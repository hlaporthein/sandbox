module.exports = function(grunt) {

	grunt.initConfig({
		sass: {                              // Task
			dist: {                            // Target
				options: {                       // Target options
		style: 'expanded'
				},
				files: {                         // Dictionary of files
					'styles.css': 'styles.scss'
				}
			}
		}
	});

	grunt.loadNpmTasks('grunt-contrib-sass');

	grunt.registerTask('default', ['sass']);

};