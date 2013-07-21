import javax.mail.Session
import javax.mail.Folder
import javax.mail.MessagingException
import javax.mail.Flags.Flag
import javax.mail.search.FlagTerm
import javax.mail.Flags


/**
 * Grrovy script to set all unread email as read
 * User: cyril
 * Date: Sep 19, 2010
 * Time: 10:05:52 PM
 * To change this template use File | Settings | File Templates.
 */


SSL_FACTORY = "javax.net.ssl.SSLSocketFactory";


username = 'xxxxxxxxxx@gmail.com'
password = 'xxxxxxxxxxxx'

def imapProps = new Properties()
[
		'mail.imap.socketFactory.class': SSL_FACTORY,
		'mail.imap.socketFactory.fallback': 'false',
		'mail.debug': 'true',
		'mail.store.protocol': 'imaps'
]



class MyGmail {
	String username
	String password

	def props
	def session
	def store
	def folder


	def init() {
		session = Session.getDefaultInstance(props, null);
		store = session.getStore("imaps");
		store.connect("imap.gmail.com", username, password);
	}

	def openFolder(String folderName) throws Exception {
		init()
		folder = store.getDefaultFolder()
		folder = folder.getFolder(folderName)

		if (folder == null) {
			throw new Exception("Invalid folder");
		}

		try {
			folder.open(Folder.READ_WRITE);
		} catch (MessagingException ex) {
			folder.open(Folder.READ_ONLY);
		}
	}


	def getUnreadMessages() {
		folder.search(new FlagTerm(new Flags(Flag.SEEN), false))
	}

	def closeFolder() {
		if (!folder.isOpen())
			folder.close(false)
	}

	def closeSession() {
		closeFolder()
		store.close()
	}

}


def myGmail = new MyGmail()
myGmail.username = username
myGmail.password = password
myGmail.props = imapProps

myGmail.openFolder("Inbox")

def messages = myGmail.getUnreadMessages()

//The above is just for testing
//Comment the below and uncomment the block following
messages[0..9].reverse().each {
	println it.subject
}

//Uncomment the below block to actually set the unread messages  as read
/*
messages.reverse().each {
	println it.subject
	it.setFlag(Flag.SEEN,true)
}
*/


myGmail.closeFolder()
myGmail.closeSession()


