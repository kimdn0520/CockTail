# [CarlTail]
게임인재원 3기 Final Project

# FBXLoader / ResourceManager 
- **FBX sdk를 활용한 FBXLoader**
  - FBX Static/Skinned Mesh 불러오기
  - Boost Library의 Serialization을 이용해 FBXLoad 수행 후 자체포맷 BinaryFile로 저장 (Mesh, Material, Animation)
  
![image](https://user-images.githubusercontent.com/58064839/211225625-b363df75-24f4-4082-ba86-9badfdd46287.png)
![image](https://user-images.githubusercontent.com/58064839/211225638-f98bcbf8-796a-419e-8a53-8ab8b81be585.png)
![image](https://user-images.githubusercontent.com/58064839/211225580-f170796e-202b-470c-95f8-a65e5bf9361e.png)

# Unity Scene Parsing
- **yaml-cpp 를 활용한 유니티 씬 파싱**
  - 현재 GameObject와 Transform, BoxCollider, SphereCollider, Camera, Light 등 컴포넌트 정보를 가져올 수 있다.
  - GameObject와 컴포넌트들의 ID 값을 가져올 수 있다.
  
![image](https://user-images.githubusercontent.com/58064839/211226383-b60b2f74-d28a-431e-8f7f-4c2849fa72db.png)
![image](https://user-images.githubusercontent.com/58064839/211226390-b9c861ef-34cc-4301-98ce-f6519f0371ff.png)
![image](https://user-images.githubusercontent.com/58064839/221354055-b48cdf9e-7ef6-460c-ba4a-f285fec8f9dd.png)

# Nav Mesh / Nav Agent
- **언리얼 엔진에서 사용하는 Recast & Detour library를 활용한 NavMesh**
  - Recast & Detour Tool를 사용해서 navMesh.bin 을 뽑은뒤에 자체엔진으로 로드
  - 내장된 함수 FindPath 를 사용해서 NavAgent 컴포넌트에 적용

# Animator State 구조 구성 / Animation Blending
- **유니티의 Animator를 참고하여 State, Transtion, Condition기능을 추가해서 적용**
  - transition duration(0~1) 의 값에 따라 Animation Blending 적용
