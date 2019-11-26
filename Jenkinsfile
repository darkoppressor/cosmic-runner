pipeline {
    agent any

    stages {
        stage('build') {
            post {
                success {
                    slackSend color: 'good', message: "Build SUCCEEDED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                    sh "/home/tails/server/sendmail '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - SUCCEEDED!' '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - SUCCEEDED:\n\nCheck console output at ${env.BUILD_URL} to view the results.' 'darkoppressor@gmail.com'"
                }
                failure {
                    slackSend color: 'danger', message: "Build FAILED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                    sh "/home/tails/server/sendmail '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - FAILED!' '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - FAILED:\n\nCheck console output at ${env.BUILD_URL} to view the results.' 'darkoppressor@gmail.com'"
                }
                unstable {
                    slackSend color: 'warning', message: "Build UNSTABLE - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                    sh "/home/tails/server/sendmail '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - UNSTABLE!' '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - UNSTABLE:\n\nCheck console output at ${env.BUILD_URL} to view the results.' 'darkoppressor@gmail.com'"
                }
            }

            steps {
                slackSend message: "Build started - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"

                sh '/home/tails/build-server/cheese-engine/tools/build-system/build $(pwd) true'
            }
        }

        stage('deploy') {
            post {
                success {
                    slackSend color: 'good', message: "Deploy SUCCEEDED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                }
                failure {
                    slackSend color: 'danger', message: "Deploy FAILED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                }
                unstable {
                    slackSend color: 'warning', message: "Deploy UNSTABLE - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                }
            }

            steps {
                slackSend message: "Deploy started - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"

                sh 'python3 -m venv "${pwd}"'
                sh 'source "${pwd}/bin/activate"'
                sh 'pip3 install --upgrade pip google-auth google-auth-httplib2 google-api-python-client'

                sh 'python3 "${pwd}/.jenkins/deploy_to_google_play.py" "/home/tails/build-server/jenkins/cosmic_runner_google_play_service.json" "org.cheeseandbacon.cosmicrunner" "${pwd}/development/android/app/build/outputs/bundle/release/app-release.aab"'
            }
        }
    }
}
