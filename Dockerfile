FROM node

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY package.json /usr/src/app/
COPY package-lock.json /usr/src/app/

RUN npm install

COPY ./src/ /usr/src/app/src
COPY ./public/ /usr/src/app/public
COPY bsconfig.json /usr/src/app/

CMD [ "npm", "start" ]