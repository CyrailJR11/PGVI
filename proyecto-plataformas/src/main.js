// ============================================================
// Crea el món — Plataformes 2D amb Phaser 3 + Tiled
// ============================================================

const config = {
  type: Phaser.AUTO,
  width: 1280,
  height: 768,
  parent: undefined,
  backgroundColor: '#5c94fc',
  physics: {
    default: 'arcade',
    arcade: {
      gravity: { y: 800 },
      debug: false // pon esto a true si quieres ver las cajas de colisión
    }
  },
  scene: { preload, create, update }
};

new Phaser.Game(config);

// Variables de la escena
let jugador, cursors, ground, monedas, meta, perills;
let comptadorText, missatgeText;
let monedesRecollides = 0;
let totalMonedes = 0;
let jocAcabat = false;

function preload() {
  // 1) Imagen del tileset (debe coincidir con la que exportaste desde Tiled)
  this.load.image('tiles', 'assets/tileset.png');

  // 2) Mapa exportado en JSON desde Tiled (Fichero -> Exportar como -> JSON)
  this.load.tilemapTiledJSON('mapa', 'assets/mapa.json');

  // 3) Spritesheet del jugador (32x48 por fotograma)
  this.load.spritesheet('jugador', 'assets/jugador.png', {
    frameWidth: 32,
    frameHeight: 48
  });

  // 4) Icono de la moneda coleccionable
  this.load.image('moneda', 'assets/moneda.png');
  this.load.image('fondo','assets/fondo.png');
}

function create() {
  jocAcabat = false;
  monedesRecollides = 0;
  this.add.image(640,384,'fondo').setDisplaySize(1280,768).setScrollFactor(0).setDepth(-10);

  // ---------- Mapa y capas ----------
  const map = this.make.tilemap({ key: 'mapa' });
  // El primer parámetro DEBE ser el nombre del tileset tal y como lo llamaste en Tiled
  const tileset = map.addTilesetImage('Sprite-0001', 'tiles');

  map.createLayer('Background', tileset, 0, 0);
  ground = map.createLayer('Ground', tileset, 0, 0);
  const foreground = map.createLayer('Foreground', tileset, 0, 0);
  foreground.setDepth(5); // que se dibuje por encima del jugador

  // Los tiles marcados con la propiedad "collides: true" en Tiled se vuelven sólidos
  ground.setCollisionByProperty({ collides: true });

  // ---------- Capa de objetos (punto de inicio, monedas, meta, peligros) ----------
  const objectLayer = map.getObjectLayer('Objects');
  if (!objectLayer) {
    console.warn('No se ha encontrado ninguna capa de objetos llamada "Objects" en el mapa.');
  }
  const objetos = objectLayer ? objectLayer.objects : [];

  const inicio = objetos.find(o => o.name === 'start') || { x: 64, y: 64 };
  const objetosMeta = objetos.filter(o => o.name === 'goal');
  const objetosMonedas = objetos.filter(o => o.name === 'coin');
  const objetosPeligro = objetos.filter(o => o.name === 'hazard');

  // ---------- Jugador ----------
  jugador = this.physics.add.sprite(inicio.x, inicio.y, 'jugador');
  jugador.setCollideWorldBounds(true);
  jugador.setDepth(10);
  this.physics.add.collider(jugador, ground);

  // Animaciones (spritesheet clásico de 9 fotogramas: 0-3 izquierda, 4 quieto, 5-8 derecha)
  if (!this.anims.exists('izquierda')) {
    this.anims.create({
      key: 'izquierda',
      frames: this.anims.generateFrameNumbers('jugador', { start: 0, end: 3 }),
      frameRate: 10,
      repeat: -1
    });
    this.anims.create({
      key: 'quieto',
      frames: [{ key: 'jugador', frame: 4 }],
      frameRate: 20
    });
    this.anims.create({
      key: 'derecha',
      frames: this.anims.generateFrameNumbers('jugador', { start: 5, end: 8 }),
      frameRate: 10,
      repeat: -1
    });
  }

  // ---------- Cámara ----------
  this.cameras.main.setBounds(0, 0, map.widthInPixels, map.heightInPixels);
  this.physics.world.setBounds(0, 0, map.widthInPixels, map.heightInPixels);
  this.cameras.main.startFollow(jugador, true, 0.08, 0.08);

  // ---------- Controles ----------
  cursors = this.input.keyboard.createCursorKeys();

  // ---------- Monedas coleccionables ----------
  monedas = this.physics.add.staticGroup();
  objetosMonedas.forEach(o => {
    const m = monedas.create(o.x, o.y, 'moneda');
    m.setDepth(4);
  });
  totalMonedes = objetosMonedas.length;

  this.physics.add.overlap(jugador, monedas, recogerMoneda, null, this);

  // ---------- Peligros (rectángulos invisibles que hacen reiniciar) ----------
  perills = this.physics.add.staticGroup();
  objetosPeligro.forEach(o => {
    const zona = this.add.zone(o.x + o.width / 2, o.y + o.height / 2, o.width, o.height);
    this.physics.add.existing(zona, true); // true = cuerpo estático
    perills.add(zona);
  });
  this.physics.add.overlap(jugador, perills, tocarPeligro, null, this);

  // ---------- Meta ----------
  meta = this.physics.add.staticGroup();
  objetosMeta.forEach(o => {
    const zona = this.add.zone(o.x + o.width / 2, o.y + o.height / 2, o.width || 32, o.height || 64);
    this.physics.add.existing(zona, true);
    meta.add(zona);
  });
  this.physics.add.overlap(jugador, meta, llegarMeta, null, this);

  // ---------- Interfaz (HUD) ----------
  comptadorText = this.add.text(12, 10, `Monedas: 0 / ${totalMonedes}`, {
    fontFamily: 'monospace',
    fontSize: '18px',
    color: '#ffffff',
    stroke: '#000000',
    strokeThickness: 3
  }).setScrollFactor(0).setDepth(20);

  missatgeText = this.add.text(640, 384, '', {
    fontFamily: 'monospace',
    fontSize: '32px',
    color: '#ffff66',
    stroke: '#000000',
    strokeThickness: 5
  }).setOrigin(0.5).setScrollFactor(0).setDepth(21);
}

function update() {
  if (jocAcabat) return;

  const velocidad = 200;

  if (cursors.left.isDown) {
    jugador.setVelocityX(-velocidad);
    jugador.flipX = true;
    jugador.anims.play('izquierda', true);
  } else if (cursors.right.isDown) {
    jugador.setVelocityX(velocidad);
    jugador.flipX = false;
    jugador.anims.play('derecha', true);
  } else {
    jugador.setVelocityX(0);
    jugador.anims.play('quieto', true);
  }

  // Salto: solo si está tocando el suelo (evita saltos infinitos en el aire)
  if (cursors.up.isDown && jugador.body.blocked.down) {
    jugador.setVelocityY(-500);
  }

  // Si se cae fuera del mapa por un agujero, reinicia el nivel
  if (jugador.y > 384 + 64) {
    reiniciarNivel(this);
  }
}

function recogerMoneda(jugador, moneda) {
  moneda.destroy();
  monedesRecollides++;
  comptadorText.setText(`Monedas: ${monedesRecollides} / ${totalMonedes}`);
}

function tocarPeligro() {
  reiniciarNivel(this);
}

function llegarMeta() {
  if (jocAcabat) return;
  jocAcabat = true;
  jugador.setVelocity(0, 0);
  jugador.body.moves = false;
  missatgeText.setText('¡Nivel completado!');
}

function reiniciarNivel(scene) {
  if (jocAcabat) return;
  scene.scene.restart();
}
