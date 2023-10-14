#include "Updater.h"
#include "ui_Updater.h"

Updater::Updater(QWidget *parent) : QWidget(parent), m_ui(new Ui::Updater)
{
  m_ui->setupUi(this);
  m_ui->info->setText(tr("Search for updates"));
  m_ui->progressBar->setHidden(true);
  m_updateChecker = m_updateChecker.create();

  connect(m_updateChecker.get(), &UpdateChecker::replyError,
          this, &Updater::replyError);
  connect(m_updateChecker.get(), &UpdateChecker::needUpdate,
          this, &Updater::needUpdate);

  m_updateChecker->checkUpdate("KoromeloDev", "MindMate");
}

Updater::~Updater()
{
  delete m_ui;
}

QString Updater::convert(quint64 bytes)
{
  if (bytes < 1024)
  {
    return QString::number(bytes) + "B";
  }

  QVector<QString> suffixList = {"KB", "MB", "GB"};
  quint8 i = 0;

  for (; i < suffixList.count(); ++i)
  {
    bytes /= 1024;

    if (bytes < 1024)
    {
      break;
    }
  }

  return QString::number(bytes) + suffixList[i];
}

void Updater::replyError(QString error)
{
  m_ui->info->setText(tr("No updates found"));
  emit openMainWindow();
}

void Updater::needUpdate(QVector<QUrl> urlList)
{
  m_ui->info->setText(tr("Updates found"));
  quint8 i = 0;
  bool found = false;
  QString platformKey;

  #if defined(Q_OS_WIN)
  platformKey = "windows";
  #elif defined(Q_OS_LINUX)
  platformKey = "linux";
  #endif

  for (const QUrl &url : urlList)
  {
    if (url.toString().toLower().contains(platformKey))
    {
      found = true;
      break;
    }

    ++i;
  }

  if (found)
  {
    m_updateDownloader = m_updateDownloader.create();
    m_ui->info->setText(tr("Start downloading"));
    m_ui->progressBar->setValue(0);
    m_ui->progressBar->setHidden(false);

    connect(m_updateDownloader.get(), &UpdateDownloader::downloadCompleted,
            this, &Updater::downloadCompleted);
    connect(m_updateDownloader.get(), &UpdateDownloader::downloadFailed,
            this, &Updater::downloadFailed);
    connect(m_updateDownloader.get(), &UpdateDownloader::downloadProgress,
            this, &Updater::downloadProgress);

    m_updateDownloader->download(urlList[i]);
    return;
  }

  emit openMainWindow();
}

void Updater::downloadCompleted(const QString &filePath, quint64 totalSize)
{
  m_ui->info->setText(tr("Downloads completed"));
  m_extractor = m_extractor.create(filePath);
  QString otputPath = QCoreApplication::applicationDirPath() + "/New Version";

  if(!QDir(otputPath).exists())
  {
    QDir(otputPath).mkdir(otputPath);
  }

  m_extractor->setOutputDirectory(otputPath);

  connect(m_extractor.get(), &QArchive::DiskExtractor::error,
          this, &Updater::extractorError);
  connect(m_extractor.get(), &QArchive::DiskExtractor::finished,
          this, &Updater::extractorFinished);
  connect(m_extractor.get(), &QArchive::DiskExtractor::progress,
          this, &Updater::extractorProgress);

  m_ui->info->setText(tr("Extracting the archive"));
  m_ui->progressBar->setValue(0);
  m_extractor->start();
}

void Updater::downloadFailed()
{
  m_ui->info->setText(tr("Download failed"));
  emit openMainWindow();
}

void Updater::downloadProgress(quint64 received, quint64 total)
{
  QString receivedString = convert(received);
  QString totalString = convert(total);
  m_ui->info->setText(tr("Downloading") + ": " + receivedString + "/" +
                      totalString);
  m_ui->progressBar->setValue(received);
  m_ui->progressBar->setMaximum(total);
}

void Updater::extractorError(short errorCode)
{
  m_ui->info->setText(tr("Extracting failed"));
  emit openMainWindow();
}

void Updater::extractorFinished()
{
  emit openMainWindow();
}

void Updater::extractorProgress(QString file, int processedEntries,
     int totalEntries, qint64 bytesProcessed, qint64 bytesTotal)
{
  QString receivedString = convert(bytesProcessed);
  QString totalString = convert(bytesTotal);
  m_ui->info->setText(tr("Extracting") + ": " + receivedString + "/" +
                      totalString);
  m_ui->progressBar->setValue(bytesProcessed);
  m_ui->progressBar->setMaximum(bytesTotal);
}
