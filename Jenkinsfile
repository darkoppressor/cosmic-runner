pipeline {
    agent any

    stages {
        stage('build') {
            steps {
                sh "./ciCd ${env.JOB_NAME} ${env.BUILD_NUMBER} ${env.BUILD_URL}"
            }
        }
    }
}
