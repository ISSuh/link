# nLink

## ToDo


### Documents
  - Architecture 및 design 문서 작성
  - 각 API 문서 작성
  - 가이드 문서 작성

### Build
  - 전반적인 CMake 스크립트 정리
  - Platform 별 빌드 구조 구현
  - Define 적용 

### Implement
  - 각종 버그 수정
  - 임시 구현으로 raw pointer 로 구현된 member와 parameter를 smart pointer로 수정

##### Base
  - Callback, Bind 이 꼭 필요한지 검토
  - task runner 개선
  - Logger 구조 검토
  - Base64, GZIP, XML 과 같은 utility 구현 혹은 적용

##### Handle
  - Handle이 꼭 필요한지 검토

##### Module
  - module remove 시 memory leak 개선 
 
##### IO
  - ASIO 제거
  - Socket pending write, read 처리
  - Socket 의 callback, buffer 등 copy 되지 않도록 처리
  - IP 대신 Domain Name 으로 접속가능하도록 개선
  - Openssl 적용
  - Shared Memory 구현
  - Pipe 구현

##### Net
  - HTTPS 구현
  - HTTP 개선
  - URI parse 구현 및 개선
  - ORM 구현

##### Component
  - task runner 적용
  - Shared Memory IPC, Pipe IPC component 구현

##### Node
  - Module Running을 위한 middleware 구현 필요

### Test
  - google test를 통한 테스트 환경 적용
  - 각 구현별 테스트코드 작성